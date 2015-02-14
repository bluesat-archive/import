#!/usr/bin/perl -w
my $desig_col = 2;
my $value_col = 6;
my $file = $ARGV[0]||die "No csv given, format: \"THIS_SCRIPT CSV_NAME\"";
open(my $data, '<',$file) or die "can\'t open selected csv file: $ARGV[0]\n";
while(my $line_org = <$data>)
{
	$line = $line_org;
	$line =~ s/^\s+|\s+$//;
	$line =~ s/\"\"/\" \"/g;
	$line = substr($line,1);
	$line = substr($line,0,-1);
	my @sections = split(/\",\"/,$line);
	if ($sections[$desig_col]!~/^\W*$/ && $sections[$value_col]!~/^\W*$/)
	{
		my $old_desig = $sections[$desig_col];
		my $old_value = $sections[$value_col];
		my @desig = split(/,/,$sections[$desig_col]);
		my @value = split(/,/,$sections[$value_col]);
		if ((scalar(@desig) eq scalar(@value)) && (scalar(@desig) gt 1))
		{
			my %hash_parts = ();
			foreach my $pos (0..(scalar(@desig)-1))
			{	
				$val = $value[$pos];
				$val =~ s/^\s+|\s+$//;
				$des = $desig[$pos];
				$des =~ s/^\s+|\s+$//;
				if(exists $hash_parts{$val})
				{
					$hash_parts{$val} = "$hash_parts{$val} $des";
				}
				else
				{
					$hash_parts{$val} = $des;
				}

			}
			foreach my $key (keys %hash_parts){
				my $new_line = $line_org;
				$new_line =~ s/$old_desig/$hash_parts{$key}/;
				$new_line =~ s/$old_value/$key/;
				print $new_line;
			}
		}
		else
		{
			print $line_org;
		}
	}
	else
	{
		print $line_org;
	} 
}
