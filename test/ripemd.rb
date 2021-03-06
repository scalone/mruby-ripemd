# RMD160 test

if Object.const_defined?(:RMD160)
  assert("Digest module") do
    Digest.class == Module
  end

  assert("RMD160") do
    RMD160.class == Class
  end

  assert("hexdigest must work") do
    msg = Digest::RMD160.hexdigest("something")
    assert_equal(msg, "f152359131ecb9fc397fb4bac2f4bb1b9b90b55f")
  end
end
