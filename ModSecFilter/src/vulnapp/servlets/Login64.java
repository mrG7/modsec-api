package vulnapp.servlets;

import java.io.IOException;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.tomcat.util.buf.Base64;

/**
 * Servlet implementation class Login64
 */
public class Login64 extends HttpServlet {
	private static final long serialVersionUID = 1L;
    private static final String DEST_PAGE = "/Common/dest.jsp";

    /**
     * @see HttpServlet#HttpServlet()
     */
    public Login64() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		String username = (String) request.getParameter("username");
		String password = (String) request.getParameter("password");
		String decodedString=Base64.base64Decode(username);
		request.setAttribute("decoded", decodedString);
		RequestDispatcher dispatcher = getServletContext().getRequestDispatcher(DEST_PAGE);
		dispatcher.forward(request,response);
	}

}
