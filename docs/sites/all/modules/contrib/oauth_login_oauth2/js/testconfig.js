function testConfig(testUrl) {
	var myWindow = window.open(testUrl, "TEST OAuth Client", "scrollbars=1 width=800, height=600");
}
function resetConfig()
{
	var myWindow = window.open("configure_oauth?&action=delete", "TEST OAuth Client", "scrollbars=1 width=800, height=600");
}