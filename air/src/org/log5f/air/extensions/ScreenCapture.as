package org.log5f.air.extensions
{
	import flash.display.BitmapData;
	import flash.display.Loader;
	import flash.display.Stage;
	import flash.external.ExtensionContext;
	import flash.system.Capabilities;

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
		
		public function capture(width:int=-1, height:int=-1):Object
		{
			width = width < 0 ? Capabilities.screenResolutionX : width;
			height = height < 0 ? Capabilities.screenResolutionY : height;
			
			var bmd:BitmapData = new BitmapData(width, height);
			
			var result:Object = this.context.call("capture", bmd);
			
			return bmd;
		}
	}
}