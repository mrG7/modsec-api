package vulnapp.filters;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.Enumeration;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.catalina.filters.Constants;

import vulnapp.modsecurity.wrappers.ModSecurityWrapper;

import java.net.URLEncoder;

/**
 * Servlet Filter implementation class ModSecurityFilter
 */
public class ModSecurityFilter implements Filter {

	private String configFilePath;
	private boolean ModSecEngineRunning;
	private ModSecurityWrapper waf;
    /**
     * Default constructor. 
     */
    public ModSecurityFilter() {
    	ModSecEngineRunning = false;
    	configFilePath = "";
    }

	/**
	 * @see Filter#destroy()
	 */
	public void destroy() {
		System.out.println("Destroying filter");
		if(ModSecEngineRunning){
			waf.stopEngine();
		}
	}
	/*
	 * Response Wrapper ( in case of blocked requests)
	 */

	private String buildRequest(HttpServletRequest httpreq)
	{
		/**
		 * Initializing String
		 */
		
		StringBuilder rawHttpRequest = new StringBuilder(vulnapp.modsecurity.wrappers.Constants.MODSEC_HEADERS_TITLE);
		
		/**
		 * Appending headers
		 */
		
		rawHttpRequest.append(httpreq.getMethod())								
					  .append(" ").append(httpreq.getRequestURI());
		
		if(httpreq.getQueryString() != null)
		{
			rawHttpRequest.append("?").append(httpreq.getQueryString());
		}
		
		rawHttpRequest.append(" ")
						.append(httpreq.getProtocol())
						.append("\n");
			
		for(Enumeration e = httpreq.getHeaderNames(); e.hasMoreElements();)
		{	
			String element =e.nextElement().toString();	
			Enumeration v=httpreq.getHeaders(element);
			rawHttpRequest.append(element).append(":")
							.append(v.nextElement().toString())
							.append("\n");		
		}
		
		/**
		 * Initialing request body
		 */
		
		rawHttpRequest.append(vulnapp.modsecurity.wrappers.Constants.MODSEC_BODY_TITLE);
		
		/**
		 * Appending POST body parameters
		 */
		
		for(Enumeration pnames = httpreq.getParameterNames(); pnames.hasMoreElements(); )
		{  
			try 
			{
				String parameter = pnames.nextElement().toString();	
				String value = URLEncoder.encode(httpreq.getParameter(parameter),"UTF-8");
				rawHttpRequest.append(parameter).append("=").append(value);
			}
			catch (UnsupportedEncodingException e) 
			{
				e.printStackTrace();
				continue;
			}
			
			if(pnames.hasMoreElements())
			{
				 rawHttpRequest.append("&");	 
			} 
		}

		return rawHttpRequest.toString();
	}
	
	/**
	 * @see Filter#doFilter(ServletRequest, ServletResponse, FilterChain)
	 */
	
	public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) 
						throws IOException, ServletException {
		
		HttpServletRequest httpreq = (HttpServletRequest) request;
		HttpServletResponse httpresp = (HttpServletResponse) response;

		int status = -1;
		
		if( httpreq instanceof HttpServletRequest &&
			httpresp instanceof HttpServletResponse){
			
			String requestMethod = httpreq.getMethod();
			System.out.println("Request method:" + requestMethod);
			
			if("GET".equals(requestMethod) || "POST".equals(requestMethod))
			{
				/*
				 * Build Raw Request
				 */
				
				String rawRequest = buildRequest(httpreq);	
				System.out.println("==========\n"+rawRequest+"\n==========");
				
				/*
				 * Start Mod Security Engine
				 */
				
			    waf = new ModSecurityWrapper();
				if(!ModSecEngineRunning){
				    System.out.println("Starting Engine");
					waf.startEngine(configFilePath);
					ModSecEngineRunning = true;
				}
				
				/*
				 * Processing Request
				 */
				
				boolean requestAction = true;
				System.out.print("Processing Request\n");
				requestAction = waf.processRequest(getConfigFilePath() , rawRequest);
				
				if(requestAction){
					System.out.println("Request Accepted");
					
				}
				else{
					System.out.println("Request Denied");
                    httpresp.sendError(HttpServletResponse.SC_FORBIDDEN);
                    return;
				}

			}
		}
		chain.doFilter(request, response);
	}
	
	public String getConfigFilePath() {
		return configFilePath;
	}

	public void setConfigFilePath(String configFilePath) {
		this.configFilePath = configFilePath;
	}

	/**
	 * @see Filter#init(FilterConfig)
	 */
	
	public void init(FilterConfig fConfig) throws ServletException {
	
		System.out.println("Initializing Filter");
		setConfigFilePath(fConfig.getInitParameter("configFilePath"));
		
	}

}
