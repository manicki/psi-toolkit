require 'rubygems'
require 'celerity'
require 'test/unit'

require 'config'

class PsiServerTest < Test::Unit::TestCase

  def test_server_address
      assert_nothing_raised(NativeException) do
      browser = Celerity::Browser.new
      browser.goto $server_address
      browser.close
    end
  end

  def test_if_list_of_publications_exists
    browser = Celerity::Browser.new
    assert_nothing_raised(NativeException) do
      browser.goto $server_address + '/publications.html'
    end

    assert browser.ul(:id => 'publications').exists?

    browser.close
  end

  def test_if_download_site_works
    browser = Celerity::Browser.new
    assert_nothing_raised(NativeException) do
      browser.goto $server_address + '/download.html'
    end

    browser.div(:id => 'downloads').uls.each do |ul|
      ul.as.each { |link| assert link.href.start_with?('http://mrt.wmi.amu.edu.pl:8080/job') }
    end

    browser.close
  end

  def test_if_bindings_site_works
    browser = Celerity::Browser.new
    assert_nothing_raised(NativeException) do
      browser.goto $server_address + '/bindings.html'
    end

    assert browser.h2s.map(&:text).join(' ').include? 'Perl'
    assert browser.h2s.map(&:text).join(' ').include? 'Python'

    browser.close
  end

end
