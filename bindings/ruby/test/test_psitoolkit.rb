require "psitoolkit"
require "test/unit"

class TestPSIToolkitWrapper < Test::Unit::TestCase

  def test_simple
    psi = ::PSIToolkit::PipeRunner.new("tp-tokenizer --lang pl")

    output = psi.run('Pan prof. dr hab. Jan Nowak.')
    expected_output = "Pan\nprof.\ndr\nhab.\nJan\nNowak\n.\n"

    assert_equal output, expected_output
  end

end
