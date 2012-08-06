package vulnapp.modsecurity.wrappers;

public class ModSecurityWrapper {
	
	private native void 
	wrapFilterRequest(String config, String event);
	
	public static boolean 
	processRequest( String configPath, 
					String eventFilePath){
		int status = 0;
		
		// Process Request Here
		
		if( status == 0) return false;
		
		return true;
	}

}
