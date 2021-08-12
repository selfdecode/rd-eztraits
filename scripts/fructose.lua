        -- This scriptlet was automatically
        -- generated from
        -- scripts/fructose.txt

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

        if rs1800546 == "C/G" then
          comment =  "Variant Carrier"
          return risk, comment
        end

        if rs76917243 == "G/T" then
          comment =  "Variant Carrier"
          return risk, comment
        end

        if rs78340951 == "C/G" then
          comment =  "Variant Carrier"
          return risk, comment
        end

        comment = "Tolerant to Fructose"
