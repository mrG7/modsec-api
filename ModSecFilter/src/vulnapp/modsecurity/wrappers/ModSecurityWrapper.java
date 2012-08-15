package vulnapp.modsecurity.wrappers;

public class ModSecurityWrapper {
	
	/* JNI Wrappers */
	private native int 
	wrapInitModSecEngine(String config);
	
	private native int
	wrapFilterRawRequest(String config, String rawRequest);
	
	private native int
	wrapTerminateModSecEngine();
	
	public boolean 
	processRequest( String configPath, String rawRequest){
		int status = 0;
				
		status = this.wrapFilterRawRequest(configPath, rawRequest);
		
		System.out.println("Status:" + status);
		
		if( status == Constants.DECLINED ) return true;

		return false;
	}
	
	public void
	startEngine(String config)
	{
		this.wrapInitModSecEngine(config);
	}
	
	public void
	stopEngine()
	{
		this.wrapTerminateModSecEngine();
	}
	
	static{
		/* Load shared C library */
		System.loadLibrary("modsecurity");
	}
	
}
