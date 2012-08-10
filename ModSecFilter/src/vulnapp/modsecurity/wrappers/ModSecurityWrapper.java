package vulnapp.modsecurity.wrappers;

public class ModSecurityWrapper {
	
	/* JNI Wrappers */
	private native int 
	wrapFilterRequest(String config, String event);
	
	private native int
	wrapFilterRawRequest(String config, String rawRequest);
	
	public boolean 
	processRequest( String configPath, 
					String rawRequest){
		int status = 0;
				
		// Process Request Here
		status = new ModSecurityWrapper().wrapFilterRawRequest(configPath, 
															rawRequest);
		
		System.out.println("Status:" + status);
		
		if( status == Constants.DECLINED ) return true;

		return false;
	}
	
	static{
		/* Load shared C library */
		System.loadLibrary("modsecurity");
	}
	
}
