# V2 OSM and map matching

V2 loads small `.osm` XML fixtures offline. It accepts `node`, `way`, `highway`, `oneway=yes`, `oneway=-1`, `maxspeed`, `name`, `access`, and `motor_vehicle`. Inaccessible motor-vehicle ways are excluded. Each way segment becomes an Edge; length is Haversine distance and time is length / speed.

Default engineering speeds (km/h): motorway 110, trunk 90, primary 70, secondary 55, tertiary 45, residential 30, service 15. These are educational assumptions, not production navigation data.

Map matching brute-forces road segments. It projects the GPS point to each local segment, scores lateral distance plus optional heading difference, and returns the best snapped point. Sequence matching applies a lightweight continuity penalty when a way changes. Complexity is O(points × edges); no PBF, R-tree, KD-tree, real-time traffic, multithreading, or GUI is included.

Example: `navcore_match --osm data/sample_map.osm --gps data/sample_trace.csv`.
