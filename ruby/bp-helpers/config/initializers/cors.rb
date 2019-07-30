# Made a custom endpoint to allow switching between users without logging/out, needs cors config
# normally this is handled by nginx but for local stuff this is needed

if defined?(Rack::Cors) && Rails.env.development?
  Rails.application.config.middleware.insert_before 0, Rack::Cors do
    allow do
      origins 'localhost:9000'
      resource '*', headers: :any, methods: %i(get post put patch delete options head)
    end
  end
end