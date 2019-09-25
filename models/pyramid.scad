//linear_extrude(height = 2.5)
/*polygon([[8,0],[-8,0],[0,16]]);
rotatepolygon([[8,0],[0,16],[16,20]]);
polygon([[-8,0],[0,16],[-16,20]]);*/

translate([-8,-8,0]) square(16);
//polygon([[16,0],[16,16],[30,8]]);
//polygon([[0,16],[16,16],[8,30]]);
//polygon([[0,0],[0,16],[-14,8]]);
//polygon([[0,0],[16,0],[8,-14]]);

translate([0,16,0]) polygon([[-8,-8],[8,-8],[0,8]]);
translate([16,0,0]) polygon([[-8,8],[-8,-8],[8,0]]);
translate([-16,0,0]) polygon([[8,-8],[8,8],[-8,0]]);
translate([0,-16,0]) polygon([[-8,8],[8,8],[0,-8]]);
