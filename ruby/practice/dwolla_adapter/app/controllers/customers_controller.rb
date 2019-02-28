class CustomersController < ApplicationController
  def create
    render status: :ok, location: "/dummy-location"
  end
end