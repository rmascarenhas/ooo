require_relative 'test_helper'

class TestPeriodic < MiniTest::Unit::TestCase

  def setup
    @p = UrlTracker::Periodic.new
  end

  def teardown
    @p.stop
  end

  def test_every_with_symbol_argument
    assert_equal 60, @p.every(:minute) {}
  end

  def test_every_with_hour_symbol
    assert_equal 60*60, @p.every(:hour) {}
  end

  def test_two_minutes
    assert_equal 2*60, @p.every(2, :minutes) {}
  end

  def test_two_hours
    assert_equal 2*60*60, @p.every(2, :hours) {}
  end

  def test_invalid_first_argument
    assert_raises(RuntimeError) do
      @p.every(:invalid) {}
    end
  end

  def test_invalid_time_unit
    assert_raises(RuntimeError) do
      @p.every(2, :invalid) {}
    end
  end

  def test_add_named_task
    @p.task("dummy").every(:minute) {}
    assert_equal ["dummy"], @p.named_tasks
  end

  def test_remove_named_task
    @p.task("dummy").every(:minute) {}
    @p.remove_task("dummy")

    assert_empty @p.named_tasks
  end

  def test_remove_inexistent_task
    assert_raises(RuntimeError) do
      @p.remove_task("dummy")
    end
  end

  def test_restart
    @p.stop
    assert !@p.running?

    @p.restart
    assert @p.running?
  end

  def test_running
    assert @p.running?, 'Expected Periodic to be running when `stop` was not called.'
  end

  def test_running_when_asked_to_stop
    @p.stop

    assert !@p.running?, 'Expected Periodic to stop'
    @p.restart
  end

end
