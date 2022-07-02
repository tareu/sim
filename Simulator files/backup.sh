cp -r ../../Current ../../Old
backup_dir=$(date +'%c')
mv ../../Old/Current ../../Old/"${backup_dir}"
