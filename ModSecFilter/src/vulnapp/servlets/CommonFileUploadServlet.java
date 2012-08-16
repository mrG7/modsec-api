package vulnapp.servlets;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.util.Iterator;
import java.util.List;
 
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
 
import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileUploadException;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
import org.apache.tomcat.util.buf.Base64;
 
 
public class CommonFileUploadServlet extends HttpServlet {
	
	private static final String TMP_DIR_PATH = "/tmp";
	private static final String DEST_DIR_PATH = "/home/varrunr/tomcat/demo/upload";
	private static final String DEST_PAGE = "/Common/dest.jsp";
	private File tmpDir;
	private File destinationDir;
	
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
	   
		
		tmpDir = new File(TMP_DIR_PATH);
		if(!tmpDir.isDirectory()) {
			throw new ServletException(TMP_DIR_PATH + " is not a directory");

		}	
		
		destinationDir = new File(DEST_DIR_PATH);
		
		PrintWriter out = response.getWriter();
	    
		response.setContentType("html");
	    out.println("<h1>Servlet File Upload Example using Commons File Upload</h1><body>");
	    out.println();

		DiskFileItemFactory  fileItemFactory = new DiskFileItemFactory ();
		
		/*
		 *Set the size threshold, above which content will be stored on disk.
		 */
		
		fileItemFactory.setSizeThreshold(1*1024*1024); //1 MB
		
		/*
		 * Set the temporary directory to store the uploaded files of size above threshold.
		 */
		
		fileItemFactory.setRepository(tmpDir);
 
		ServletFileUpload uploadHandler = new ServletFileUpload(fileItemFactory);
		try {
			/*
			 * Parse the request
			 */
			List items = uploadHandler.parseRequest(request);
			Iterator itr = items.iterator();
			
			while(itr.hasNext()) {
			
				FileItem item = (FileItem) itr.next();
				
				/*
				 * Handle Form Fields.
				 */
				
				if(item.isFormField()) {
					
					String decodedString=Base64.base64Decode(item.getString());
					
					out.println("File Name = "+item.getFieldName()+", Value = "+decodedString);
				} else {
					
					/*out.println("Field Name = "+item.getFieldName()+
						", File Name = "+item.getName()+
						", Content type = "+item.getContentType()+
						", File Size = "+item.getSize());
					*/
					/*
					 * Write file to the ultimate location.
					 */
					
					File file = new File(destinationDir,item.getName());
					item.write(file);
				}
				out.println("</body>");
				out.close();
			}
			
			//RequestDispatcher dispatcher = getServletContext().getRequestDispatcher(DEST_PAGE);
			//dispatcher.forward(request,response);
			
		}catch(FileUploadException ex) {
			log("Error encountered while parsing the request",ex);
		} catch(Exception ex) {
			log("Error encountered while uploading file",ex);
		}	
	}
}