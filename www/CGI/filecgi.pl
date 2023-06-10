#! /usr/bin/perl

use CGI;


my ($form) = new CGI;

sub trim {
    my $str = shift;
    $str =~ s/^\s+|\s+$//g;
    return $str;
}

print $form->header;
print $form->start_html(-title => 'Infos page');
print $form->h1("Please enter your Full Name & Addresse");
if ($form->param){
    my $fname = ($form->param('fullName'));
    my $addre = ($form->param('address'));
    $fname = trim($fname);
    $fname = trim($addre);
    if (length($fname) > 0 && length($addre) > 0){
        print $form->h3("Hello Mr $fname,");
        print $form->h3("Your new Adress in this form now is : $addre");
    }
}
else {
    print $form->start_form(
        -method => 'GET',
        -action => '',
    );
    print 'FULL NAME : ';
    print $form->textfield(
        -type => 'text',
        -label => 'Full name :',
        -name => 'fullName',
        -size => '30',
    );
    print 'ADDRESS : ';
    print $form->textfield(
        -type => 'text',
        -label => 'Address :',
        -name => 'address',
        -size => '32',
    );
    print $form->submit(
        -name=> 'submit',
        -value=> 'send',
    );
    print $form->end_form;
}


print $form->end_html;

exit (0);
