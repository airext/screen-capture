package org.log5f.air.extensions
{
	import flash.display.BitmapData;
	import flash.external.ExtensionContext;

	public class ScreenCapture
	{
		public function ScreenCapture()
		{
			this.context = 
				ExtensionContext.createExtensionContext("org.log5f.air.extensions.ScreenCapture", null);
		}
		
		private var context:ExtensionContext;
		
		public function isSupported():Boolean
		{
			return this.context.call("isSupported") as Boolean;
		}
		
		public function capture():Object
		{
			var bmd:BitmapData = new BitmapData(1920, 1080);
			
			var result:Object = this.context.call("capture", bmd);
			
			trace("result:", result);
			
			return bmd;
		}
	}
}