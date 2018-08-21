////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018 Leo Tenenbaum
// This file is part of GraphColoring.
//
// GraphColoring is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GraphColoring is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GraphColoring.  If not, see <https://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

#include "value.hpp"

#include <algorithm>
#include <sstream>

#include "utils/errors.hpp"

namespace graphcoloring {

std::map<std::string, Value::operation_t> Value::operation_table;

Value::Value() : type(Type::NULL_TYPE) {}

Value::Value(int val_)
{
	val = val_;
	type = Type::VALUE;
}

Value::Value(std::vector<Value> list_)
{
	list = list_;
	type = Type::LIST;
}

Value::Value(const std::string& string)
{
	FromString(string);
}

void Value::FromString(const std::string& string)
{
	const std::map<std::string, Type> type_map =
	{
			{"", Type::NULL_TYPE},
			{"V", Type::NUMBER_OF_VERTICES},
			{"E", Type::NUMBER_OF_EDGES},
			{"vertices", Type::VERTICES},
			{"edges", Type::EDGES}
	};
	if (type_map.count(string))
	{
		type = type_map.at(string);
		return;
	}
	try
	{
		val = std::stoi(string);
		type = Type::VALUE;
	}
	catch (std::invalid_argument&)
	{
		// NOTE: Does not check if variable exists! This means that errors
		// will only be caught when the variable is evaluated, but it allows
		// out-of-order declaration.
		type = Type::VARIABLE;
		variable_name = string;
	}
}


Value::Value(pugi::xml_node node)
{
	std::string name = node.name();
	if (name == "op")
	{
		std::string v1 = node.attribute("val1").value();
		std::string v2 = node.attribute("val2").value();
		std::string op = node.attribute("op").value();
		type = Type::OPERATION;
		val1 = std::shared_ptr<Value>(new Value(v1));
		val2 = std::shared_ptr<Value>(new Value(v2));
		ReadOperation(op);
	}
	else if (name == "var")
	{
		FromString(node.attribute("val").value());
	}
	else if (name == "map" || name == "fold")
	{
		if (name == "map")
			type = Type::MAP;
		else
			type = Type::FOLD;
		std::string op = node.attribute("op").value();
		std::string v = node.attribute("val").value();
		std::string start = node.attribute("start").value();
		if (v == "")
			utils::errors::Die("No argument passed to " + name + ".");
		ReadOperation(op);
		val1 = std::shared_ptr<Value>(new Value(v));
		fold_start = std::shared_ptr<Value>(new Value(start));
	}
	else if (name == "zip")
	{
		type = Type::ZIP;
		std::string op = node.attribute("op").value();
		std::string v1 = node.attribute("val1").value();
		std::string v2 = node.attribute("val2").value();
		if (v1 == "" || v2 == "")
			utils::errors::Die("Empty argument passed to zip.");
		ReadOperation(op);
		val1 = std::shared_ptr<Value>(new Value(v1));
		val2 = std::shared_ptr<Value>(new Value(v2));
	}
	else
	{
		utils::errors::Die("Unknown tag: " + name);
	}
}

Value::operation_t Value::SimpleOperation(std::function<int(int,int)> op)
{
	return [op](const Graph&, int a, int b) { return op(a, b); };
}

Value::operation_t Value::SimpleBoolOperation(std::function<bool(int,int)> op)
{
	return [op](const Graph&, int a, int b) { return op(a, b) ? 1 : 0; };
}

Value::operation_t Value::SimpleLogicOperation(std::function<bool(bool,bool)>op)
{
	return [op](const Graph&, int a, int b) {
		return op(a != 0, b != 0) ? 1 : 0;
	};
}

void Value::InitializeOperationTable()
{
	operation_table = {
			{"min", SimpleOperation([](int a, int b)->int{return std::min(a,b);})},
			{"max", SimpleOperation([](int a, int b)->int{return std::max(a,b);})},
			{"+", SimpleOperation(std::plus<int>())},
			{"-", SimpleOperation(std::minus<int>())},
			{"*", SimpleOperation(std::multiplies<int>())},
			{"/", SimpleOperation(std::divides<int>())},
			{"%", SimpleOperation(std::modulus<int>())},
			{"=", SimpleBoolOperation(std::equal_to<int>())},
			{"!=", SimpleBoolOperation(std::not_equal_to<int>())},
			{"<", SimpleBoolOperation(std::less<int>())},
			{">", SimpleBoolOperation(std::greater<int>())},
			{"<=", SimpleBoolOperation(std::less_equal<int>())},
			{">=", SimpleBoolOperation(std::greater_equal<int>())},
			{"and", SimpleLogicOperation(std::logical_and<bool>())},
			{"or", SimpleLogicOperation(std::logical_or<bool>())},
			{"not", SimpleLogicOperation([](bool a, bool b)->bool{
				return  !a; // Ignore second argument.
			})},
			{"v1", [](const Graph& graph, int e, int)->int {
				return graph.GetEdgeByIDConst(e).from.id;
			}},
			{"v2", [](const Graph& graph, int e, int)->int {
				return graph.GetEdgeByIDConst(e).to.id;
			}},
			{"connected", [](const Graph& graph, int v1, int v2)->int {
				if (!graph.HasVertexWithID(v1) || !graph.HasVertexWithID(v2))
					return 0;
				return graph.HasEdge(v1, v2) ? 1 : 0;
			}},
			{"degree", [](const Graph& graph, int v, int)->int {
				return graph.Degree(v);
			}},
			{"vertex-color", [](const Graph& graph, int v_id, int)->int {
				const Vertex& v = graph.GetVertexByIDConst(v_id);
				return (int)v.Color();
			}},
			{"edge-color", [](const Graph& graph, int e_id, int)->int {
				const Edge& e = graph.GetEdgeByIDConst(e_id);
				return (int)e.Color();
			}}
	};
}

void Value::ReadOperation(std::string op)
{
	if (operation_table.size() == 0)
		InitializeOperationTable();
	if (operation_table.count(op))
	{
		operation = operation_table[op];
	}
	else
	{
		utils::errors::Die("Invalid operation: " + op);
	}
}

std::vector<int> Value::EvalListOperation(const Graph& graph,
	std::function<Value(std::string)> lookup_variable) const
{
	switch (type)
	{
	case Type::VARIABLE:
		return lookup_variable(variable_name)
				.EvalListOperation(graph, lookup_variable);
	case Type::VERTICES:
	{
		std::vector<int> list;
		for (const Vertex* v : graph.vertices)
		{
			list.push_back(v->id);
		}
		return list;
	}
	case Type::EDGES:
	{
		std::vector<int> list;
		for (const Edge* e : graph.edges)
		{
			list.push_back(e->id);
		}
		return list;
	}
	case Type::LIST:
	{
		std::vector<int> out;
		for (Value v : list)
			out.push_back(v.Eval(graph, lookup_variable));
		return out;
	}
	case Type::MAP:
	{
		std::vector<int> in = val1->EvalListOperation(graph, lookup_variable);
		std::vector<int> out;
		for (int v : in)
			out.push_back(operation(graph, v, 0));
		return out;
	}
	case Type::ZIP:
	{
		std::vector<int> in1= val1->EvalListOperation(graph, lookup_variable);
		std::vector<int> in2= val2->EvalListOperation(graph, lookup_variable);
		std::vector<int> out;
		for (int i = 0; i < (int)std::min(in1.size(),in2.size()); i++)
			out.push_back(operation(graph, in1[i], in2[i]));
		return out;
	}
	default: break;
	}
	std::stringstream s;
	s << "Invalid list operation: " << (int)type << ".";
	utils::errors::Die(s.str());
	return std::vector<int>();
}

int Value::Eval(const Graph& graph,
	std::function<Value(std::string)> lookup_variable) const
{
	switch (type)
	{
	case Type::NULL_TYPE:
		return 0;
	case Type::VALUE:
		return val;
	case Type::VARIABLE:
		return lookup_variable(variable_name).Eval(graph, lookup_variable);
	case Type::NUMBER_OF_VERTICES:
		return graph.V();
	case Type::NUMBER_OF_EDGES:
		return graph.E();
	case Type::OPERATION:
		assert(operation);
		return operation(graph,
						 val1->Eval(graph, lookup_variable),
						 val2->Eval(graph, lookup_variable));
	case Type::FOLD:
	{
		assert(operation);
		int x = fold_start->Eval(graph, lookup_variable);
		for (const Value& v : val1->EvalListOperation(graph, lookup_variable))
		{
			x = operation(graph, x, v.Eval(graph, lookup_variable));
		}
		return x;
	}

	default: break;
	}
	std::stringstream s;
	s << "Invalid operation: " << (int)type << ".";
	utils::errors::Die(s.str());
	return -1;
}

} // namespace graphcoloring
