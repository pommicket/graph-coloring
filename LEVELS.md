# Writing Levels for GraphColoring
The `assets/levels` folder contains [XML](https://en.wikipedia.org/wiki/XML) files which describe levels.

If you would like to submit a level, please create a pull request.
## `level-list.xml`
The file `assets/levels/level-list.xml` contains a list of level categories and their levels in XML format. It should be relatively clear how to add a new level or category. Note that you will need to delete your `saves` directory or update the version number in `level-list.xml` if you change it for the application to update the levels once you close and re-open it.
## Level XML format overview
In the `assets/levels` directory, there is one folder for each category. Each level is stored as a .xml file in its category's directory and has the following nodes:
- `<level title="" description="" objective=""/>` - This will control the text displayed at the top of the level. You can create multi-line descriptions with `;` as a delimiter.
- `<global-vertex-protections protect=""/>` and `<global-edge-protections protect=""/>` - If you add protections to these, all of the vertices/edges in the graph node will have those protections.
- `<colors>...</colors>` - A list of colors the level will be using.
- `<graph>...</graph>` - A description of the starting graph
- `<rules>...</rules>` - Restrictions on the graph.
- `<values>...</values>` - Variables, most importantly `points` and `objective`.
- `<path>...</path>` and `<cycle>...</cycle>` - Paths and cycles
## Colors
The `<colors>` node contains `<color>` children nodes. Each child node defines a color name. For example, the node `<color color="#FF0000" name="red"/>` will allow you to use "red" for vertex colors.

Each `<color>` node can have the following attributes:
- `color` (e.g. `color="#FF0000"`) (required) - This attribute's value is the hex code of the color. It is in hexadecimal format and can be either #RRGGBB or #RRGGBBAA.
- `name` (e.g. `name="red"`) (required) - This is the name which will stand in for the color when creating vertices.
- `vertex-points` (e.g. `vertex-points="3"`) - This is the number of points you will get for creating a vertex with this color. Defaults to 0.
- `edge-points` (e.g. `edge-points="-1"`) - This is the number of points you will get for creating an edge with this color. Defaults to 0.

## Graph
The `<graph>` has `<vertex>` and `<edge>` children.
### Vertices
Each `<vertex>` node describes a vertex and can have the following attributes:
- `x`, `y` (e.g. `x="450" y="350"`) (required) - The x- and y-coordinates of this vertex.
- `id` (e.g. `id="top-left"`) - The ID of this vertex. Used for defining edges and in the `<values>` node. If left empty, the vertex will not have an ID.
- `color` (e.g. `color="red"`)- The color of this vertex. Defaults to the first defined color.
- `origin` (e.g. `origin="t"`) - If a vertex has this attribute, it will be a connectedness origin (i.e. it will be filled in at the start). There can be multiple origins. The value of this attribute is ignored.
- `protect` (e.g. `protect="cde"`) - The protections for this vertex. See Protections section for how to format these.
### Edges
Each `<edge>` node describes an edge and can have the following attributes:
- `v1`, `v2` (e.g. `v1="top-left" v2="top-right"`) (required) - The IDs of the vertices which this edge connects.
- `id` (e.g. `id="top-edge"`) - The ID of this edge. Used in the `<values>` node. If left empty, the edge will not have an ID.
- `color` (e.g. `color="red"`) - The color of this edge. Defaults to the first defined color.
- `protect` (e.g. `protect="cd"`) - The protections for this edge. See Protections section for how to format these.
### Protections
Protections can be added to any `<vertex>` or `<edge>` using the `protect` attribute. Vertices and edges can have these protections:
- `c` - Protect the color of the vertex (it will not be editable by the user).
- `d` - Protect the vertex from being deleted.
- `e` - (Vertices only) Disallow the user from adding an edge to/from the vertex.

The `protect` attribute's value is a string containing the characters for all of its protections, e.g. `protect="cd"` for protecting the color and protecting deletion.
#### Global Protections
Global protections are defined in the `<global-vertex-protections>` and `<global-edge-protections>` nodes. Both of them can have a `protect` attribute which will apply those protections to all the vertices or edges. There is one additional protection, `a`, which protects new vertices or edges from being added.

For example, `<global-vertex-protections protect="ac">` will protect new vertices from being added, and prevent coloring vertices.
## Rules
The rules node contains the following types of children:
- `<connected/>` - This node has no attributes and just specifies that the graph must be connected.
- `<edge-` or `<vertex-` `minimum>` or `maximum>` - These are bound rules which specify a minimum or maximum number of vertices or edges.
- `<edge-` or `<vertex-` `rule>` - These rules describe which colors vertices and edges can be.
### Bound rules
Bound rules can be one of four nodes: `<edge-minimum>`, `<edge-maximum>`, `<vertex-minimum>`, or `<vertex-maximum>`. Bound rules limit the number of vertices or edges of a color. For example, `<vertex-maximum color="red" max="7"/>` will restrict the number of red vertices to a maximum of 7.
#### Any and same
If the `color` attribute is `any` (which it defaults to), this will restrict the total number of vertices or edges. For example, `<vertex-maximum color="any" max="3"/>` will restrict the total number of vertices to 3.

If the `color` attribute is `same`, this will restrict all colors at once. For example `<vertex-maximum color="same" max="3"/>` will mean that the user cannot create more than 3 of any color of vertex.
### Edge rules
`<edge-rule>` nodes bans edges with the given endpoint and edge colors. The node has the following attributes:
- `v1` - The color of the first vertex.
- `edge` - The color of the edge.
- `v2` - The color of the second vertex.

All these attributes default to `any`.

For example, `<edge-rule v1="red" edge="red" v2="red"/>` will ban edges from red vertices to red vertices with red edges.
#### Any and same
If any color is specified as `any`, it will be banned for all colors.

For example, `<edge-rule v1="red" edge="any" v2="red"/>` will ban all edges from red vertices to red vertices, regardless of the edge color.

If any color is specified as `same`, it will be banned for all colors, but if there are multiple uses of `same`, there is the additional requirement that the two `same`s are the same color.

For example, whereas `<edge-rule v1="any" edge="any" v2="any"/>` bans all edges, `<edge-rule v1="same" edge="any" v2="same"/>` bans edges from any two vertices of the same color.

## Values
The following nodes can be children of the `<values>` node:
- `<comment>` - A comment. For most other nodes, invalidly-named nodes will be ignored, but this is not the case for `<values>`.
- `<include>` - Include values from another file. The file name can be specified through the `file` attribute (e.g., `<include file="includes/foo">` will include `assets/levels/includes/foo.xml`).
- `<var>` - Variables. The format of this node is described below.
- `<op>` - Value-based operations. The format of this node is described below.
- `<map>`, `<zip>`, and `<fold>` -  These are operations on lists which are described below.

Note that the variables are evaluated lazily, which means that they do not have to be defined in any particular order.

Only integer values are supported, so for booleans, `0` and `1` are used.
### Initial variables
The variables `E` and `V` are pre-defined to be the number of edges and vertices in the graph. `vertices` and `edges` are the lists of vertices and edges.
### Variables
Variables can be created using the `<var>` node, with the `id` and `val` attributes, for example
```xml
<var id="a" val="5"/>
<var id="b" val="E"/>
```
Will create the variables `a` and `b`, where the value of `a` will be 5, and the value of `b` will be the number of edges
### Points and objective
`points` and `objective` are important variables because they define how many points the user has and what the objective is (i.e., what the points are out of).
### Operations
Operations have the following properties:
- `id` - The variable to output to.
- `op` - The operation to apply.
- `val1` - The first argument to the operation.
- `val2` - The second argument to the operation.
For example,
```xml
<var id="a" val="5"/>
<var id="b" val="7"/>
<op id="c" val1="a" op="+" val2="b"/>
```
Will set `c` to 12.
Here is a list of currently-supported operations:

**Integer operations**
- `min` - The minimum of two integers.
- `max` - The maximum of two integers.
- `+` - Plus
- `-` - Minus
- `*` - Times
- `/` - Divided by
- `%` - val1 mod val2

**Boolean operations**
- `=` - True (1) if val1 = val2, false (0) otherwise
- `!=` - Not equal to - False iff val1 = val2.
- `<` - Less than.
- `>` - Greater than.
- `<=` - Less than or equal to.
- `>=` - Greater than or equal to.
- `and` - True iff both val1 and val2 are true.
- `or` - True iff val1 (inclusive) or val2 is true.
- `not` - True iff val1 is not true. Ignores val2.

**Graph operations**
- `v1` - Given an edge ID as val1, returns the first vertex of the edge.
- `v2` - Given an edge ID as val1, returns the second vertex of the edge.
- `connected` - Given two vertex IDs, returns if they are connected by an edge.
- `degree` - Given a vertex ID, returns the degree of the vertex.
- `vertex-color` - Given a vertex ID, returns the color of the vertex.
- `edge-color` - Given an edge ID, returns the color of the edge.

### Lists
Lists are also supported. At the moment, all lists are just `vertices` or `edges` with operations applied to them.
#### map
This is similar to the `map` Haskell function.
Given an operation and a list, it returns a list with the operation applied to each element. The element is passed as val1. For example,
```xml
<map op="vertex-color" id="colors" val="vertices"/>
```
Will create a list of the colors of each vertex.

#### zip
This is similar to the `zipWith` Haskell function.

`zip` combines two lists into one list using an operation.

For example,
```xml
<zip op="+" val1="a" val2="b" id="c"/>
```
Will create a list `c` such that `c[i] = a[i] + b[i]` for all `i`.


#### fold
This is similar to the `foldl` Haskell function.

`fold` folds up a list starting at the first element, applying the operation each time.
The starting value of the accumulation can be specified with the `start` attribute (defaults to 0).

For example,
```xml
<fold op="*" start="1" val="list" id="product"/>
```
Will find the product of everything in the list (e.g., `[1,2,3] -> 6`).

## Paths and cycles
`<path>` nodes and `<cycle>` nodes are identical in their syntax except for the name. Changing the name to `cycle` just enforces that the first vertex is the same as the last one. Path nodes use point accumulators. The number of points starts at the given value, and changes each time it passes through an edge. The starting value can be specified using the `points` attribute, and defaults to 0.

Each path consists of edge nodes. Each edge node has a color, operation, and value. Here is an example from the fifth level in the Eulerian Paths & Cycles category:
```xml
<path points="0">
    <edge color="red" op="+" val="1"/>
    <edge color="blue" op="*" val="2"/>
</path>
```
This says that the number of points starts at 0, and is increased by 1 every time is passees through a red edge, and doubles every time it passes through a blue edge.
