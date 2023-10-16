use strict;
use warnings;
use Text::CSV;
use Clipboard;

my $kRejectStepSize = 320e-9;

main();

sub main {
    my $clean = 01;
    my %channels;

    my @rows = @{Text::CSV::csv(in => 'D:\Scratch~~\PerlScratch\delme13.csv')};

    shift @rows; # Remove header

    for my $row (@rows) {
        my ($channel, $time, $value) = @$row;

        next if $time < 0;

        push @{$channels{$channel}{rows}}, [$time, $value];
    }

    if ($clean) {
        clean($channels{$_}{rows}) for keys %channels;
    }

    my @results;
    my $min;
    my $max;

    for my $channel (sort keys %channels) {
        my $index = 0;

        push @{$results[$index++]}, @$_ for @{$channels{$channel}{rows}};

        $min = $results[$index-2][-1] if !defined($min) || $results[$index-2][-1] < $min;
        $max = $results[$index-1][-1] if !defined($max) || $results[$index-1][-1] > $max;
    }

    my $valueCount = @{$results[0]};
    my $text;

    $text .= join "\t", map {$_, ''} sort keys %channels;
    $text .= "\n";
    $text .= join "\t", map {(0, $min)} keys %channels;
    $text .= "\n";
    $text .= join "\t", map {(1e-9, $max)} keys %channels;
    $text .= "\n";

    my $lines = 0;

    for my $result (@results) {
        last if @$result < $valueCount;
        $text .= join "\t", @$result;
        $text .= "\n";
        ++$lines;
    }

    Clipboard->copy($text);
    print $text;
    print "Lines: ", $lines + 1, "\n";
}


sub clean {
    my ($rows) = @_;

    my $normOffset = 0;
    my $lastValue;
    my $min;
    my $max;

    # Remove discontinuities
    for my $row (@$rows) {
        my ($time, $value) = @$row;

        $lastValue //= $value;
        $value -= $normOffset;

        my $valueDelta = $value - $lastValue;

        if (abs($valueDelta) >= $kRejectStepSize) {
            $normOffset += $valueDelta;
            $value -= $valueDelta;
        }

        $row->[1] = $value;
        $min = $value if !defined($min) || $value < $min;
        $max = $value if !defined($max) || $value > $max;
        $lastValue = $value;
    }

    # Normalise
    $_ -= $lastValue for $min, $max;
    $_->[1] -= $lastValue for map {$_} @$rows;

    push @$rows, [@{$rows->[-1]}, $rows->[-1][-2] + 0.001, $min];
    push @$rows, [@{$rows->[-1]}, $rows->[-1][-1] + 0.001, $max];

    # Remove wild values during startup
    while (@$rows) {
        last if abs($rows->[0][1]) < 1e-3;
        shift @$rows;
    }
}
