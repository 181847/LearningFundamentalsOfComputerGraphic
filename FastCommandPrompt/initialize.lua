print("prepare lua")

clipboard = require("clipboard")

function GetNormalComment()
	date = os.date('%Y/%m/%d')
	user = 'liuyang28'
	comment = '/**\r\n* ['..date..' '..user..'] \r\n*/';
	return comment;
end