.PHONY: rocksdb_lib_sources.txt
rocksdb_lib_sources.txt:
	$(MAKE) -C rocksdb rust-support UPDATE_REPO=0
	cp rocksdb/rust-rocksdb-src.txt $@

gen_lib_sources: rocksdb_lib_sources.txt

gen_statistics: rocksdb/monitoring/statistics.cc
	bash gen_statistics.bash $< Ticker     > ../src/statistics_enum_ticker.rs
	bash gen_statistics.bash $< Histogram  > ../src/statistics_enum_histogram.rs
