
#!/usr/bin/env python3

import cgi

# print("Content-Type: text/html")
# print()

form = cgi.FieldStorage()

name = form.getfirst("name", "Unknown")

# print("<html>")
# print("<head>")
# print("<title>Hello, {}!</title>".format(name))
# print("</head>")
# print("<body>")
# print("<h1>Hello, {}!</h1>".format(name))
# print("</body>")
# print("</html>")
import os

# Get the QUERY_STRING environment variable
query_string = os.environ.get("QUERY_STRING")

# Split the QUERY_STRING into its components
query_components = query_string.split("&")

# Get the value of the "name" parameter
for component in query_components:
    name, value = component.split("=")
    if name == "name":
        user_name = value
        break

print ("Content-Type : text/html\r\n\r\n")  
print ()
print ("<html>")  
print ("<head>")  
print ("<title>Python CGI {} </title>". format(user_name))  
print ("<head>")  
print ("<body>")  
print ("<h1>my first Python CGI </h1>")  
print ("</body>")  
print ("</html>")  