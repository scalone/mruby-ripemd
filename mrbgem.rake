MRuby::Gem::Specification.new('mruby-ripemd') do |spec|
  spec.license = 'MIT'
  spec.authors = 'Thiago Scalone'

  spec.cc.include_paths << "#{build.root}/src"
end
