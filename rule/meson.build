sources = files(
	'ch_rule_group.c',
	'ch_rule_match.c',
	'ch_rule.c',
	'ch_rule_pool.c',
	'ch_rule_engine_context.c',
	'ch_rule_engine.c'
)

librule = library('rule',sources,dependencies:[common,cjson])
rule = declare_dependency(link_with: librule,
	include_directories: include_directories('.'))
