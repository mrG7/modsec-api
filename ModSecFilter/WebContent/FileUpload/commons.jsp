<html>
	<head></head>
	<body>
		<p>Upload file Example</p>
		<form action="/ModSecFilter/upload" enctype="multipart/form-data" method="POST">
			<input type="text" name = "username" ><br>
		    <input type="text" name ="password" ><br>
			<input type="file" name="file1" ><br>
			
			<input type="Submit" value="Upload File"><br>
		</form>
	</body>
</html>