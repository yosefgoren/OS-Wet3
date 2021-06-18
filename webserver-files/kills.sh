echo "before:"
ps -aux | grep "[/]server 2020"
kill $(ps -aux | grep "[/]server 2020" | awk '{print $2}')
echo "after:"
ps -aux | grep "[/]server 2020"
