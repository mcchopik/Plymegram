use strict;
use warnings;

sub crc32 { my ($d)=@_; my $c=0xffffffff; for my $b (unpack('C*',$d)) { $c^=$b; for(1..8){$c=($c&1)?(0xedb88320^($c>>1)):($c>>1)} } return $c^0xffffffff }
sub chunk { my ($t,$d)=@_; return pack('N',length($d)).$t.$d.pack('N',crc32($t.$d)) }

# Generate a fully black BMP (164x314, 24-bit) for the finished page.
my ($w,$h,$path)=(164,314,'/home/userland/tg/Telegram/build/wizard-finished.bmp');
my $row=(($w*3+3)&~3); my $off=54; my $size=$off+$row*$h;
my $bmp=pack('a2VVV','BM',$size,0,$off);
$bmp .= pack('V',40) . pack('l<l<',$w,$h) . pack('vv',1,24);
$bmp .= pack('V',0) for 1..9;
my $data='';
for my $y (0..$h-1){ $data .= ("\0" x ($w * 3)) . ("\0" x ($row - $w * 3)) }
my @rows; for(my $i=0;$i<$h;$i++){push @rows,substr($data,$i*$row,$row)}
$bmp.=join('',reverse @rows);
open(my $f,'>:raw',$path) or die $!; print $f $bmp; close $f;
print "wrote $path\n";
