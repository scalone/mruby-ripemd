# RMD160 test

if Object.const_defined?(:RMD160)
  assert("RMD160") do
    RMD160.class == Module
  end

  assert("hexdigest must work") do
    msg = RMD160.hexdigest("something")
    assert_equal(msg, "76202357772b26db74cf62e36efb6d69f68e48ab")
  end
end
