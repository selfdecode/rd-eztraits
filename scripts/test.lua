
	
	if rs568149713 == "A/G" and rs557514207 == "G/G" then
		comment = "high risk"
		risk = 0.8
		return risk, comment
	end
		
	
	if chr1:16949=="A/C" or rs533090414=="C/C" then
		comment = "medium risk"
		risk = 0.5
	end


