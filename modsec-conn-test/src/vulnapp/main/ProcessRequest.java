package vulnapp.main;

import vulnapp.modsecurity.wrappers.ModSecurityWrapper;

public class ProcessRequest {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		if(args.length < 2){
			System.out.println("Enter at least 2 args");
			return;
		}
		
		ModSecurityWrapper waf = new ModSecurityWrapper();
		
		System.out.println("Processing request");
		boolean status = waf.processRequest(args[0], args[1]);
		
		if( status){
			System.out.println("Accepted");
		}
		else System.out.println("Denied");
	}

}
