        -- This scriptlet was automatically
        -- generated from
        -- scripts/fructose.txt

        if (rs1800546 == "C/C") and (rs76917243 == "G/G") and (rs78340951 == "G/G" or rs78340951 == "A/G" or rs78340951 == "A/A") then
          comment =  "Tolerant to Fructose"
          return risk, comment
        end

        if rs1800546 == "C/G" then
          comment =  "Possibly intolerant to Fructose"
          return risk, comment
        end

        if rs76917243 == ""/GT then
          comment =  "Possibly intolerant to Fructose"
          return risk, comment
        end

        if rs78340951 == "C/G" then
          comment =  "Possibly intolerant to Fructose"
          return risk, comment
        end

        comment = "Fructose Intolerant"
