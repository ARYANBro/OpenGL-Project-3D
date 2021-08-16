newaction
{
	trigger = "clean",
	description = "Cleans built files",

	onWorkspace = function(wks)
		printf("Cleaning workspace %s", wks.name)

		os.remove("Bin/**")
		os.remove("Obj/**")
	end,

	onEnd = function()
		print("Done.")
	end

}