def check_pid(pid)
    Process.kill(0, pid) rescue false
end
