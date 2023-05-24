#!/usr/bin/env python

import cgi

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from fields
name = form.getvalue('name')
email = form.getvalue('email')

# Set the content type of the response
print("Content-type: text/html\n")

# Generate the HTML response
print("<html>")
print("<head>")
print("<title>CGI Script</title>")
print("</head>")
print("<body>")
print("<h1>CGI Script Example</h1>")
print("<p>Name: %s</p>" % name)
print("<p>Email: %s</p>" % email)
print("</body>")
print("</html>")
