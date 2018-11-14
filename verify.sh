input_dir=$1
output_dir=$2
for f in "$2"/*\.comp\.plaintext;
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
