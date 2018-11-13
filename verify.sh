# # files=$(ls txt-files/ | grep -v "\.comp\.plaintext")
# for name in txt-files/"\.comp\.plaintext"
# do
#     echo "$name"
#     # file1="txt-files/""$name"
#     # file2="txt-files/"$name".comp.plaintext"
#     # DIFF=$(cmp $file1 $file2)
#     # if [ "$DIFF" == "" ]
#     # then
#     #     echo "no differences"
#     # fi
# done

for f in txt-files/*\.comp\.plaintext;
do
    # echo "$f"
    file1="txt-files/"$(basename "$f" ".comp.plaintext")
    file2="$f"
    DIFF=$(cmp "$file1" "$file2")
    if [ "$DIFF" != "" ]
    then
        echo "difference detected comparing files"
        echo "$file1 and $file2"
        exit 1
    fi
done
