class SwitchCredentialsController < AuthTokenController
  def create
    user = User.find_by(email: params[:email])
    if user
      auth_token_validator.destroy_token
      auth_token = AuthTokenCreator.new.create_token_for_user(user.id)
      render status: :ok, json: SessionJsonBuilder.new.build(user, auth_token)
    else
      render status: :unprocessable_entity, json: {}
    end
  end
end