# EZTraits

Tool that executes a set of rules defined in lua syntax. It runs on Linux, and MacOS.

## Getting started

1. Clone the repo:  
git clone https://github.com/selfdecode/rd-eztraits.git  

3. type:  
   cd rd-eztraits  
   make  

## Test it

./test_EZTraits

## Run it

The program needs two inputs:  

(1) A VCF or CSV file with the genotype calls (see data/sample.vcf and data/sample.calls in the repo).  

Use the -i option to point the program to the call file. In addition, specify the rs id column using the -rs option, the -pos option for the chromosome position (has to follow the convention "chr1:16949"), and the -call option for the genotype (has to follow the convention "A/G", where the order of calls matters, and has to be consistent with the lua rules!). Note that at least the rs ids or positions are required; you can have both, yet either one is sufficient.  

(2) A lua-type set of rules (see scripts/test.lua in the repo).

If you are not familiar with lua, check out this quick guide: http://tylerneylon.com/a/learn-lua/

Lua lets you define any set of rules and any hierarchy, for example:

	if rs568149713 == "A/G" and chr1:16949 == "G/G" then
		comment = "high risk"
		risk = 0.8
	end

Here, you can address the genotypes by their names (rs id or chromosome position). The string variable 'comment' is reserved for comments, the floating point variable risk is the risk factor. You can define hierarchies either by if - then - else, or intert the line 'return risk, comment' if you are done, e.g.:

	if rs568149713 == "A/G" and rs557514207 == "G/G" then
		comment = "high risk"
		risk = 0.8
		return risk, comment
	end

will not evaluate the rest of the script. 

Notes:  
~= is the not equal operator  
You can use the () brackets as well to define herarchies.  


(3) Alternatively, you can define rules in free text, e.g. for fructose intolerance, the rules can be written like this:

	All
	rs1800546 'CC'
	rs76917243 'GG'
	rs78340951 'GG' or 'AG' or 'AA'
	= "Tolerant to Fructose"
	Any:
	rs1800546 'CG'
	rs76917243 'GT'
	rs78340951 'CG'
	== "Possibly intolerant to Fructose"
	Any:
	rs1800546 'GG'
	rs76917243 'TT'
	rs78340951 'CC'
	== "Fructose Intolerant"

EZTraits accepts and interprets the keywords ‘All’, ‘Any’, and ‘else’, optionally followed by a colon. Acceptable genotype call formats include ‘CG’ and C/G (with optional single quotation marks), where the latter convention has to be used for sites that contain indels, e.g. ‘T/TGAT’.  

Running ./Txt2Lua turn above text into this lua snippet:  


        -- This scriptlet was automatically
        -- generated from
        -- scripts/fructose.txt

        if (rs1800546 == "C/C") and (rs76917243 == "G/G") and (rs78340951 == "G/G" or rs78340951 == "A/G" or rs78340951 == "A/A") then
          comment =  "Tolerant to Fructose"
        end

        if rs1800546 == "C/G" then
          comment =  "Possibly intolerant to Fructose"
          return risk, comment
        end

        if rs76917243 == "G/T" then
          comment =  "Possibly intolerant to Fructose"
          return risk, comment
        end

        if rs78340951 == "C/G" then
          comment =  "Possibly intolerant to Fructose"
          return risk, comment
        end

        if rs1800546 == "G/G" then
          comment =  "Fructose Intolerant"
          return risk, comment
        end

        if rs76917243 == "T/T" then
          comment =  "Fructose Intolerant"
          return risk, comment
        end

        if rs78340951 == "C/C" then
          comment =  "Fructose Intolerant"
          return risk, comment
        end

And the final lua script that is internally executed is:

	function evaluate ( )

        	comment = "none"
        	risk = 0

        	rs1800546 = "G/G"
        	rs76917243 = "G/T"
        	rs78340951 = "C/C"
        	-- This scriptlet was automatically
        	-- generated from
        	-- scripts/fructose.txt

        	if (rs1800546 == "C/C") and (rs76917243 == "G/G") and (rs78340951 == "G/G" or rs78340951 == "A/G" or rs78340951 == "A/A") then
          	  comment =  "Tolerant to Fructose"
        	end

        	if rs1800546 == "C/G" then
          	  comment =  "Possibly intolerant to Fructose"
           	  return risk, comment
        	end

        	if rs76917243 == "G/T" then
          	  comment =  "Possibly intolerant to Fructose"
          	  return risk, comment
        	end

        	if rs78340951 == "C/G" then
          	  comment =  "Possibly intolerant to Fructose"
           	  return risk, comment
        	end

        	if rs1800546 == "G/G" then
          	  comment =  "Fructose Intolerant"
          	  return risk, comment
        	end

        	if rs76917243 == "T/T" then
          	  comment =  "Fructose Intolerant"
          	  return risk, comment
        	end

        	if rs78340951 == "C/C" then
          	  comment =  "Fructose Intolerant"
          	  return risk, comment
        	end


        	return risk, comment
	end



