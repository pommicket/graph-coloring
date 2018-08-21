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

#ifndef GRAPHCOLORING_LEVELS_VALUE_H_
#define GRAPHCOLORING_LEVELS_VALUE_H_


#include "pugi/pugixml.hpp"
#include "../graphs/graph.hpp"

namespace graphcoloring {

class Value {
public:
	typedef std::function<int(const Graph&,int,int)> operation_t;
	Value();
	Value(std::vector<Value> list);
	Value(int val);
	Value(const std::string& string);
	Value(pugi::xml_node node);
	virtual ~Value() {}
	int Eval(const Graph& graph,
		std::function<Value(std::string)> lookup_variable) const;
	static void AddNodeToVariables(pugi::xml_node node);
private:
	void FromString(const std::string& string);
	void ReadOperation(std::string op);
	static operation_t SimpleOperation(std::function<int(int,int)> op); // For operations which don't look at the graph.
	// Bool operations return bools.
	static operation_t SimpleBoolOperation(std::function<bool(int,int)> op);
	// Logical operations take bools and return bools.
	static operation_t SimpleLogicOperation(std::function<bool(bool,bool)> op);
	static void InitializeOperationTable();
	std::vector<int> EvalListOperation(const Graph& graph, // Handles anything that returns a list.
		std::function<Value(std::string)> lookup_variable) const;
	operation_t operation;
	static std::map<std::string, Value::operation_t> operation_table;
	std::shared_ptr<Value> fold_start;
	std::shared_ptr<Value> val1;
	std::shared_ptr<Value> val2;
	std::string variable_name;
	int val = 0;
	std::vector<Value> list;
	enum class Type
	{
		NULL_TYPE,
		VALUE, // This value represents a constant number
		LIST,
		VARIABLE,
		NUMBER_OF_VERTICES,
		NUMBER_OF_EDGES,
		VERTICES,
		EDGES,
		OPERATION,
		LIST_OPERATION,
		MAP,
		ZIP,
		FOLD
	};
	Type type;
};

} // namespace graphcoloring

#endif // GRAPHCOLORING_LEVELS_VALUE_H_
