package vulnapp.main;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

import vulnapp.modsecurity.wrappers.ModSecurityWrapper;

public class ProcessRequest {

	/**
	 * @param args
	 */
	private static String readStrFromFile(String filename)
	{
	    String str, request = "";    
		try {
		    BufferedReader in = new BufferedReader(new FileReader(filename));
		    while ((str = in.readLine()) != null) {
		        request = request.concat(str + "\n");
		    }
		    in.close();
		} catch (IOException e) {
		}
		return request;
	}
	
	public static void main(String[] args) {
		
		if(args.length < 2){
			System.out.println("Enter at least 2 args");
			return;
		}

		String request = readStrFromFile(args[1]);
		
		ModSecurityWrapper waf = new ModSecurityWrapper();
		ModSecurityWrapper waf2 = new ModSecurityWrapper();

		boolean status = waf.processRequest(args[0], request);
		//boolean status2 = waf2.processRequest(args[0], request);

		//status = waf.processRequest(args[0], request);

		if(status){
			System.out.println("Accepted");
		}
		else System.out.println("Denied");
	}

}
