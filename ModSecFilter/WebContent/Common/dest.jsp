<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Destination</title>
</head>
<body bgcolor="black">
<font color="green">
<center>
<h1>
<% String username = (String) request.getParameter("username");
String decoded = (String) request.getParameter("decoded");
%>
Hi <%=username  %> , you have reached the destination! <br/><br/>

</h1>
<img src="/ModSecFilter/Common/MOBLogo.jpg">
</center>
</font>
</body>
</html>