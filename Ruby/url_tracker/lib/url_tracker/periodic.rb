module UrlTracker

  require 'eventmachine'

  # Small class wrapping EventMachine calls to programatically
  # execute code blocks.
  class Periodic

    # maybe consider :day in the future
    TIME_UNITS = {
      minute: 60,
      minutes: 60,
      hour:   60*60,
      hours:   60*60
    }

    # Creates a new instance of UrlTracker::Periodic and starts the event loop.
    def initialize
      @named_tasks = {}
      start_event_loop
    end

    # Register a new task to be executed in a specified amount of time.
    # Examples:
    #
    #   p = UrlTracker::Periodic.new
    #   p.every(:minute)    { do_something }    #=> executed every minute
    #   p.every(2, :minutes) { do_something }   #=> executed every 2 minutes
    #   p.every(4, :hours)   { do_something }   #=> executed every 4 hours
    def every(*args, &block)
      time = 1

      case args.first
        when Integer then time  = args[0]*seconds_for(args[1])
        when Symbol  then time *= seconds_for(args[0])
        else raise "Invalid period #{args[0].inspect}"
      end

      task = { every: time, task: block }
      task.merge!(name: @name) if named_task?

      schedule_task(task)
      @name = nil

      time
    end

    # Returns named tasks registered.
    # Example
    #
    #   p = UrlTracker::Periodic.new
    #   p.task(:foo).every(:minute)     { do_something}
    #   p.task(:bar).every(2, :minute)  { do_other_thing }
    #   p.named_tasks #=> [:foo, :bar]
    def named_tasks
      task_names
    end

    # Removes a task named +name+, so that it will no longer run
    def remove_task(name)
      raise "Unregistered task #{name.inspect}" unless @named_tasks.include?(name)

      unschedule_task(name)
    end

    # Restarts the event loop
    def restart
      stop if running?
      start_event_loop
    end

    # Checks if the tasks are running
    def running?
      @event_thread.alive?
    end

    # Stop all scheduled tasks
    def stop
      @event_thread.terminate
      @event_thread.join
    end

    # Used for creating named tasks or, in other words, tasks that can be removed
    # later using #remove
    def task(name)
      @name = name.to_s
      self
    end

  private

    def named_task?
      !@name.nil?
    end

    def seconds_for(time_unit)
      raise "Unkown time unit #{time_unit.inspect}" unless TIME_UNITS.include?(time_unit)
      TIME_UNITS[time_unit]
    end

    # +task+ is expected to be in the format:
    #
    #   { every: 60, task: #<Proc:0x...> }
    #
    # for a task to be run every minute, for example.
    def schedule_task(t)
      periodic_timer = EM.add_periodic_timer(t[:every], &t[:task])
      @named_tasks[@name] = periodic_timer if named_task?
    end

    def start_event_loop
      # start the event loop in a separate thread
      @event_thread = Thread.new { EM.run }

      # Wait for the reactor to be ready
      while !@event_thread.stop?; end
    end

    def task_names
      @named_tasks.keys
    end

    def unschedule_task(name)
      EM.cancel_timer(@named_tasks.delete(name))
    end

  end

end
