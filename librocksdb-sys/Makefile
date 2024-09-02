.PHONY: rocksdb_lib_sources.txt
rocksdb_lib_sources.txt:
	$(MAKE) -C rocksdb rust-support UPDATE_REPO=0
	cp rocksdb/rust-rocksdb-src.txt $@

gen_lib_sources: rocksdb_lib_sources.txt
