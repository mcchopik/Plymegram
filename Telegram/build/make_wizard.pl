use strict;
use warnings;

sub write_bmp {
    my ($path, $w, $h, $dark) = @_;
    my $row = (($w * 3 + 3) & ~3);
    my $pixels = $row * $h;
    my $off = 54;
    my $size = $off + $pixels;
    my $bmp = pack('a2Vx x x V', 'BM', $size, $off);
    $bmp .= pack('V', 40); # DIB size
    $bmp .= pack('l<l<', $w, $h);
    $bmp .= pack('vv', 1, 24);
    $bmp .= pack('V', 0) x 5;
    $bmp .= pack('V', 0); # colors important
    $bmp .= pack('V', 0); # green mask
    $bmp .= pack('V', 0);
    $bmp .= pack('V', 0);
    $bmp .= pack('V', 0);

    my $data = '';
    for my $y (0 .. $h - 1) {
        my $line = '';
        for my $x (0 .. $w - 1) {
            my ($r, $g, $b);
            if ($dark) {
                # dark glass: deep navy/purple radial gradient with subtle neon glow
                my $dx = ($x - $w / 2) / ($w / 2);
                my $dy = ($y - $h / 2) / ($h / 2);
                my $d = sqrt($dx * $dx + $dy * $dy);
                my $glow = 1 - $d;
                $glow = 0 if $glow < 0;
                $r = int(12 + 40 * $glow * $glow);
                $g = int(10 + 20 * $glow * $glow);
                $b = int(28 + 70 * $glow * $glow);
                # neon rim
                if ($d > 0.92 && $d < 0.99) {
                    $r = 90; $g = 60; $b = 180;
                }
            } else {
                my $dx = ($x - $w / 2) / ($w / 2);
                my $dy = ($y - $h / 2) / ($h / 2);
                my $d = sqrt($dx * $dx + $dy * $dy);
                my $glow = 1 - $d; $glow = 0 if $glow < 0;
                $r = int(20 + 45 * $glow);
                $g = int(15 + 25 * $glow);
                $b = int(40 + 80 * $glow);
            }
            $r = 255 if $r > 255; $g = 255 if $g > 255; $b = 255 if $b > 255;
            $line .= pack('CCC', $b, $g, $r);
        }
        $line .= "\0" x ($row - $w * 3);
        $data .= $line;
    }
    # BMP rows bottom-up
    my @rows;
    for (my $i = 0; $i < $h; $i++) { push @rows, substr($data, $i * $row, $row) }
    $bmp .= join('', reverse @rows);
    open(my $f, '>:raw', $path) or die $!;
    print $f $bmp;
    close $f;
    print "wrote $path ($size bytes)\n";
}

write_bmp('/home/userland/tg/Telegram/build/wizard-image.bmp', 164, 314, 1);
write_bmp('/home/userland/tg/Telegram/build/wizard-small-image.bmp', 55, 55, 1);
