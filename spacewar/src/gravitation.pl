use strict;
# create a gravitational field for the screen
# 80 x 60 bytes, strongest in the center, weakest at the corners.

my @max;

open my $FILEX, '>', 'gradient-x.bin';
open my $FILEY, '>', 'gradient-y.bin';
for my $row (0..29)
{
	for my $col (0..39)
	{
		# translate to cx,cy
		my $y = 15-$row;                # cy centered
		my $x = 20-$col;                # cx centered
		my $r2 = ($x * $x + $y * $y);   # radius squared (up to something like 625)
		$r2 = 1 if $r2 == 0;
		my $h  = sqrt($r2);             # radius/hypoteneuse
		my $mass = 300;                 # mass of the sun
		my $g = 2 * $mass / $r2;            # gravity

		my $dy = int($g*$y/$h);
		my $dx = int($g*$x/$h);

		print $FILEX pack 's<', $dx;
		print $FILEY pack 's<', $dy;

        push @max, $dx, $dy;
	}
}
close $FILEX;
close $FILEY;

@max = sort {$a <=> $b} @max;

print join " ", @max;
print "\n";
