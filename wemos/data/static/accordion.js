
document.getElementById("btn-phrase").onclick = function () {
	var panel = document.getElementById('info-phrase')
	if (panel.style.maxHeight) {
		panel.style.maxHeight = null;
	} else {
		panel.style.maxHeight = panel.scrollHeight + "px"
	}
	document.getElementById('info-matrix').style.maxHeight = null;
	document.getElementById('info-predef').style.maxHeight = null;
}
document.getElementById("btn-matrix").onclick = function () {
	var panel = document.getElementById('info-matrix')
	if (panel.style.maxHeight) {
		panel.style.maxHeight = null;
	} else {
		panel.style.maxHeight = panel.scrollHeight + "px"
	}
	document.getElementById('info-phrase').style.maxHeight = null;
	document.getElementById('info-predef').style.maxHeight = null;
}
document.getElementById("btn-predef").onclick = function () {
	var panel = document.getElementById('info-predef')
	if (panel.style.maxHeight) {
		panel.style.maxHeight = null;
	} else {
		panel.style.maxHeight = panel.scrollHeight + "px"
	}
	document.getElementById('info-matrix').style.maxHeight = null;
	document.getElementById('info-phrase').style.maxHeight = null;
}
