<?hh // strict
namespace Usox\Sharesta;

interface ApplicationInterface {

	public function handle(string $base_path): void;
}
