MRuby::Gem::Specification.new('mruby-ripemd') do |spec|
  spec.license = 'MIT'
  spec.authors = ['scalone', 'sadasant']
  spec.version = '1.0.0'
  spec.summary = 'Ruby interface to C RIPEMD-160 hash-function by Antoon Bosselaers'

  spec.cc.include_paths << "#{build.root}/src"
end
