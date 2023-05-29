#! /usr/bin/perl

use CGI;

my ($form) = new CGI;

print $form->header;
print $form->start_html(-title => 'Infos page');
print $form->h1("(9ALWA)");

if ($form->param){
    print_data_form($form);
}
else {
    output_my_form($form);
}
print $form->end_html;

exit (0);

sub output_my_form {
    my ($form) = @_;

    print $form->start_form(
        -method => 'GET',
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

sub print_data_form {
    my ($form) = @_;

    my $fname = $form->param('fullName');
    my $addre = $form->param('address');

    print $form->h3("Hello Mr $fname,");
    print $form->h3("Yout new Adress in this form now is : $addre");
}