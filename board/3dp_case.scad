s = 1.165; // 3mm wall thickness 1.165
b = 2.0;
d = 3+b;

module mage()
{
    translate([-20, 0, 0])
    {
    difference()
    {
        union()
        {
            difference()
            {
                rotate([0, 0, 360/16]) scale([s, s, 1])cylinder(r=20, h=d, $fn=8);
                translate([0, 0, b]) rotate([0, 0, 360/16]) cylinder(r=20, h=d, $fn=8);
            }

            difference()
            {
                translate([40, 0, 0]) rotate([0, 0, 360/16]) scale([s, s, 1]) cylinder(r=20, h=d, $fn=8);
                translate([0, 0, b]) translate([40, 0, 0]) rotate([0, 0, 360/16]) cylinder(r=20, h=d, $fn=8);
            }
        }

        translate([0, 0, b]) translate([20, 0, d/2]) cube([20, 28, d], center=true);
    }

    difference()
    {
        translate([20, 0, d/2]) scale([s, s, 1]) cube([12, 29, d], center=true);
        translate([0, 0, b]) translate([20, 0, d/2]) cube([16, 28, d], center=true);
    }
}
}

translate([20, 18.5, 0]) color("red") cube([2, 3, d+1]);
mage();