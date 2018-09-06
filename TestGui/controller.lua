print("have recived command: "..command)

if command == 'comment' then
	if clipboard.copy(GetNormalComment()) then
		print('copy to clipboard success')
	end
end
