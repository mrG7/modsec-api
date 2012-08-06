package vulnapp.modsecurity.wrappers;

public class ModSecurityWrapper {
	
	/* JNI Wrapper */
	private native int 
	wrapFilterRequest(String config, String event);
	
	public boolean 
	processRequest( String configPath, 
					String eventFilePath){
		int status = 0;
		
		System.out.println("Setting config path: " + configPath);
		System.out.println("Setting event path: " + eventFilePath);
		
		// Process Request Here
		status = new ModSecurityWrapper().wrapFilterRequest(configPath, eventFilePath);
		
		System.out.println("Status:" + status);
		
		if( status == Constants.DECLINED ) return true;

		return false;
	}
	
	static{
		/* Load shared C library */
		System.loadLibrary("modsecurity");
	}
	
}
