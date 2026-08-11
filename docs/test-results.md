These are the Unity Unit test results:

```
test/test_register/test_register.cpp:332: test_register_external_returns_true [PASSED]
test/test_register/test_register.cpp:333: test_register_managed_returns_true [PASSED]
test/test_register/test_register.cpp:334: test_register_id_zero_fails [PASSED]
test/test_register/test_register.cpp:335: test_have_screen [PASSED]
test/test_register/test_register.cpp:336: test_begin_calls_external_only [PASSED]
test/test_register/test_register.cpp:337: test_managed_lazy_loaded [PASSED]
test/test_register/test_register.cpp:338: test_manager_destructor_deletes_managed [PASSED]
test/test_register/test_register.cpp:339: test_no_current_screen [PASSED]
test/test_register/test_register.cpp:340: test_current_screen [PASSED]
test/test_register/test_register.cpp:341: test_is_current [PASSED]
test/test_register/test_register.cpp:342: test_register_25_screens [PASSED]
test/test_register/test_register.cpp:343: test_duplicate_id [PASSED]

test/test_lifecycle/test_lifecycle.cpp:309: test_managed_screen_begin_called_on_first_load [PASSED]
test/test_lifecycle/test_lifecycle.cpp:310: test_managed_screen_instances_are_independent [PASSED]
test/test_lifecycle/test_lifecycle.cpp:311: test_begin_selects_initial_screen [PASSED]
test/test_lifecycle/test_lifecycle.cpp:312: test_request_screen_is_deferred_until_update [PASSED]
test/test_lifecycle/test_lifecycle.cpp:313: test_transition_to_unregistered_screen_is_ignored [PASSED]
test/test_lifecycle/test_lifecycle.cpp:314: test_end_called_on_transition [PASSED]
test/test_lifecycle/test_lifecycle.cpp:315: test_end_false_blocks_transition [PASSED]
test/test_lifecycle/test_lifecycle.cpp:316: test_force_transition_ignores_end_false [PASSED]
test/test_lifecycle/test_lifecycle.cpp:317: test_force_transition_still_calls_end [PASSED]
test/test_lifecycle/test_lifecycle.cpp:318: test_previous_screen [PASSED]
test/test_lifecycle/test_lifecycle.cpp:319: test_get_previous_id [PASSED]
test/test_lifecycle/test_lifecycle.cpp:320: test_previous_unchanged_when_transition_rejected [PASSED]


test/test_router/test_router.cpp:136: test_router_zero_means_no_opinion [PASSED]
test/test_router/test_router.cpp:137: test_first_router_wins [PASSED]
test/test_router/test_router.cpp:138: test_later_routers_not_called_after_resolution [PASSED]
test/test_router/test_router.cpp:139: test_router_receives_current_screen_id [PASSED]
test/test_router/test_router.cpp:173: test_fps [PASSED]

============================================================== SUMMARY ==============================================================
Environment Test Status Duration
------------- -------------- -------- ------------
uno test_router PASSED 00:00:10.780
uno test_lifecycle PASSED 00:00:10.804
uno test_register PASSED 00:00:10.419
============================================ 28 test cases: 28 succeeded in 00:00:32.003 ============================================

```