## depthmapX - multi-platform spatial network analyses software


This is the home for the development of depthmapX.


Latest releases can be found at the [releases page](https://github.com/SpaceGroupUCL/depthmapX/releases)


For any issues/bugs/crashes please create [a new issue](https://github.com/SpaceGroupUCL/depthmapX/issues/new)


For more information please check the [documentation](./docs/index.md) and the [wiki](https://github.com/SpaceGroupUCL/depthmapX/wiki)


depthmapX is a multi-platform  software platform to perform a set of spatial network analyses designed to understand social processes within the built environment. It works at a variety of scales from building through small urban to whole cities or states. At each scale, the aim of the software is to produce a map of open space elements, connect them via some relationship (for example, intervisibility or overlap) and then perform graph analysis of the resulting network. The objective of the analysis is to derive variables which may have social or experiential significance.

At the building or small urban scale, depthmapX can be used to assess the visual accessibility in a number of ways. It can produce point isovists, that is, polygons representing the visually accessible area from a location, along with measures of those polygons (such as perimeter, area and so on), or it can further join a dense grid of isovists into a visibility graph of intervisible points (with graphs of up to about 1000000 point locations). The visibility graph may then be analysed directly using graph measures, or used as the core of an agent-based analysis. In the agent-based analysis a number of software agents representing pedestrians are released into the environment. Each software agent is able to access the visual accessibility information for its current location from the visibility graph, and this informs its choice of next destination. The numbers of agents passing through gates can be counted, and compared to actual numbers of pedestrians passing through gates.

At the small to medium urban scale, depthmapX can be used to derive an ‘axial map’ of a layout. That is, derive a reduced straight-line network of the open space in an environment. The axial map has been the staple of space syntax research for many years, but the mathematical derivation of it is novel. The automatic derivation allows an objective map for research into city form and function. Once the map has been generated, it may be analysed using graph measures, and the measures may be transferred to gate layers in order to compare with indicators of pedestrian or social behaviour. For larger systems where the derivation algorithm becomes cumbersome, pre-drawn axial maps may be imported.

Axial maps may be broken into segment maps, or segment maps, such as road-centre line maps, may be imported directly. These may be analysed using a variety of techniques, such as according to angular separation, road distance, or segment steps. For example, number of shortest angular paths through a segment may be calculated, or the average road distance from each segment to all others may be calculated.

The analyses described so far are fairly fixed, however depthmapX also offers the capability of extension through two levels of interface. The first level, a scripting interface based on the Python language, allows researchers to calculate new derived measures as well as to add graph measures, such as circuit lengths, for each of the graph types. It also allows the ability to select groups of nodes according to value or according to simple algorithms. The second level, the Software Developers’ Kit (SDK) allows programmers the ability to write new forms of analysis. For example, researchers at the West Japan Railway Company have used the software developers’ kit to add new agent-based analysis where agents are directed through a station layout including ticket barriers and signposting.

depthmapX can display information as coloured maps, tables and scattergrams that comparing measures against other measures or observed data, as well as a three-dimensional view of agents walking. Data for plans can be imported from AutoCAD’s DXF format, or from Ordnance Survey NTF files or US Tiger Line maps, as well as from GIS through MapInfo’s MIF/MID format. Export may also be to MIF/MID, or to text files which may be analysed further using statistical analysis packages. Additionally, maps may be exported as vector graphics EPS files, or by copying and pasting into other software such as Microsoft Word.

##

depthmapX is using the GPLv3 licence. 

Please read http://www.gnu.org/licenses/gpl-3.0.html and also join www.jiscmail.ac.uk/lists/DEPTHMAP.html .

Thanks,
Tasos Varoudis
