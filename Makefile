TESTS = 'test/test_distribution.js'

test:
	@NODE_ENV=test expresso \
		$(TESTS)

.PHONY: test
