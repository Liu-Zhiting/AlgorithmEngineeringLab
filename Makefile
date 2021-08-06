dirs = 0-data-generator 1-sort 2-matrix-multiply 3-page-rank 4-bfs 5-triangle-counting

.PHONY: $(dirs) clean

$(dirs):
	@for dir in $(dirs); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(dirs); do \
		$(MAKE) -C $$dir clean; \
	done
