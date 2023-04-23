const char *__asan_default_options() {
  return "detect_leaks=1:alloc_dealloc_mismatch=true:strict_string_checks=true:detect_stack_use_after_return=true";
}